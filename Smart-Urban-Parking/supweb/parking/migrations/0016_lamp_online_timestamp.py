# -*- coding: utf-8 -*-
from __future__ import unicode_literals

from django.db import models, migrations


class Migration(migrations.Migration):

    dependencies = [
        ('parking', '0015_auto_20150809_1534'),
    ]

    operations = [
        migrations.AddField(
            model_name='lamp',
            name='online_timestamp',
            field=models.DateTimeField(null=True, blank=True),
        ),
    ]
