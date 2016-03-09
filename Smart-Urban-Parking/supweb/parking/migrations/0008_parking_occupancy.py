# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations
import django.core.validators


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0007_auto_20150618_1949'),
    ]

    operations = [
        migrations.AddField(
            model_name='parking',
            name='occupancy',
            field=models.IntegerField(default=1, validators=[django.core.validators.MaxValueValidator(1000), django.core.validators.MinValueValidator(1)]),
        ),
    ]
